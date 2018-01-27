<shader file="src/lighting/phong/phong.hlsl">

<techniques>
	<techique name="t1">
		<VS entryPoint="t1_VsMain">
			<Defines>
				<item DEFINE_NAME="DEFINE_VALUE"/>
			</Defines>
		
			<InputLayout>
				<item semanticName="Position" semanticIndex="0" stream="0" format="Float3"/>
				<item semanticName="Texcoord" semanticIndex="0" stream="0" format="Float2"/>
			</InputLayout>

			<InputBuffers>
				<item type="Matrix4x4"/>
			</InputBuffers>
		</VS>

		<PS entryPoint="t1_psMain">
			<Defines>
				<item name="DEFINE_NAME" val="DEFINE_VALUE"/>
			</Defines>
			
			<Rasterizer>
				<item name="FillMode" value="Solid"/>
			</Rasterizer>

			<DepthStencil>
				<item name="DepthEnable" value="true"/>
				<item name="StencilReadMask" value="255"/>
				<item name="DepthFunc" value="Greater"/>
			</DepthStencil>

			<BlendDescriptions AlphaToCoverage="true" IndependentBlend="true">
				<RT name="0">
					<item name="BlendEnable" value="true" />
					<item name="ColorSrc" value="InvDstColor" />
					<item name="ColorDst" value="SrcColor" />
					<item name="ColorOp" value="Max" />
					<item name="AlphaSrc" value="Src1Alpha" />
					<item name="AlphaDst" value="DstAlpha" />
					<item name="AlphaOp" value="RevSubstract" />
					<item name="WriteMask" value="Blue"/>
				</RT>
				<RT name="4">
					<item name="BlendEnable" value="true" />
					<item name="ColorSrc" value="InvDstColor" />
					<item name="ColorDst" value="SrcColor" />
					<item name="ColorOp" value="Max" />
					<item name="AlphaSrc" value="Src1Alpha" />
					<item name="AlphaDst" value="DstAlpha" />
					<item name="AlphaOp" value="RevSubstract" />
					<item name="WriteMask" value="Blue"/>
				</RT>
			</BlendDescriptions>

			<Outputs>
				<item RT="0" format="RGBA"/>
			</Outputs>
		</PS>
	</techique>
</techniques>
</shader>