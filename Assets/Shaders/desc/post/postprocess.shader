<shader file="src/post/postprocess.hlsl" provider="PbrEnvironmentProvider">

<techniques>
	<techique name="t1">
		<VS entryPoint="t1_VsMain" shaderModel="vs_4_0">
			<Defines>
				<item DEFINE_NAME="DEFINE_VALUE"/>
			</Defines>
		
			<InputLayout>
				<item semanticName="Position" semanticIndex="0" stream="0" format="Float3"/>
				<item semanticName="Texcoord" semanticIndex="0" stream="0" format="Float2"/>
				<item semanticName="Normal" semanticIndex="0" stream="0" format="Float3"/>
			</InputLayout>

			<InputBuffers>
				<item type="Matrix4x4"/>
			</InputBuffers>
		</VS>

		<PS entryPoint="t1_psMain" shaderModel="ps_4_0">
			<Defines>
				<item name="DEFINE_NAME" val="DEFINE_VALUE"/>
			</Defines>
			
			<Rasterizer>
				<item name="FillMode" value="Solid"/>
				<item name="CullMode" value="None"/>
			</Rasterizer>

			<DepthStencil>
				<item name="DepthEnable" value="false"/>
				<item name="WriteMask" value="MaskAll"/>
				<item name="StencilReadMask" value="255"/>
				<item name="StencilWriteMask" value="255"/>
				<item name="DepthFunc" value="Less"/>
			</DepthStencil>

			<BlendDescriptions AlphaToCoverage="false" IndependentBlend="false">
				<RT name="0">
					<item name="BlendEnable" value="false" />
					<item name="ColorSrc" value="InvDstColor" />
					<item name="ColorDst" value="SrcColor" />
					<item name="ColorOp" value="Max" />
					<item name="AlphaSrc" value="Src1Alpha" />
					<item name="AlphaDst" value="DstAlpha" />
					<item name="AlphaOp" value="RevSubstract" />
					<item name="WriteMask" value="All"/>
				</RT>
			</BlendDescriptions>

			<Outputs>
				<item RT="0" format="RGBA"/>
			</Outputs>
		</PS>
	</techique>
</techniques>
</shader>